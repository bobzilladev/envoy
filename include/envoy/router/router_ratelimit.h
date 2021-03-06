#pragma once

#include "envoy/http/filter.h"
#include "envoy/http/header_map.h"
#include "envoy/ratelimit/ratelimit.h"

namespace Router {
/**
 * Base interface for generic rate limit action.
 */
class RateLimitAction {
public:
  virtual ~RateLimitAction() {}

  /**
   * Potentially populate the descriptor array with new descriptors to query.
   * @param route supplies the target route for the request.
   * @param descriptors supplies the descriptor array to optionally fill.
   * @param local_service_cluster supplies the name of the local service cluster.
   * @param headers supplies the header for the request.
   * @param remote_address supplies the trusted downstream address for the connection.
   */
  virtual void populateDescriptors(const RouteEntry& route,
                                   std::vector<::RateLimit::Descriptor>& descriptors,
                                   const std::string& local_service_cluster,
                                   const Http::HeaderMap& headers,
                                   const std::string& remote_address) const PURE;
};

typedef std::unique_ptr<RateLimitAction> RateLimitActionPtr;

/**
 * Rate limit configuration.
 */
class RateLimitPolicyEntry : public RateLimitAction {
public:
  /**
   * @return the stage value that the configuration is applicable to.
   */
  virtual int64_t stage() const PURE;

  /**
   * @return runtime key to be set to disable the configuration.
   */
  virtual const std::string& killSwitchKey() const PURE;

  /**
   * @return the route key, if it exists.
   */
  virtual const std::string& routeKey() const PURE;
};

/**
 * Rate limiting policy.
 */
class RateLimitPolicy {
public:
  virtual ~RateLimitPolicy() {}

  /**
   * @param stage the value for finding applicable rate limit configurations.
   * @return set of RateLimitPolicyEntry that are applicable for a stage.
   */
  virtual const std::vector<std::reference_wrapper<const RateLimitPolicyEntry>>&
  getApplicableRateLimit(int64_t stage) const PURE;
};

} // Router
